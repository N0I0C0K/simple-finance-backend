const options: Intl.DateTimeFormatOptions = {
  year: 'numeric',
  month: 'numeric',
  day: 'numeric',
  hour: 'numeric',
  minute: '2-digit',
}

export const formatDateTime = (time: number): string => {
  const date = new Date(time * 1000)
  return date.toLocaleString('en-US', options)
}
